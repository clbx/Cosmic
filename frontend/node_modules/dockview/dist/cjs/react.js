"use strict";
var __assign = (this && this.__assign) || function () {
    __assign = Object.assign || function(t) {
        for (var s, i = 1, n = arguments.length; i < n; i++) {
            s = arguments[i];
            for (var p in s) if (Object.prototype.hasOwnProperty.call(s, p))
                t[p] = s[p];
        }
        return t;
    };
    return __assign.apply(this, arguments);
};
var __read = (this && this.__read) || function (o, n) {
    var m = typeof Symbol === "function" && o[Symbol.iterator];
    if (!m) return o;
    var i = m.call(o), r, ar = [], e;
    try {
        while ((n === void 0 || n-- > 0) && !(r = i.next()).done) ar.push(r.value);
    }
    catch (error) { e = { error: error }; }
    finally {
        try {
            if (r && !r.done && (m = i["return"])) m.call(i);
        }
        finally { if (e) throw e.error; }
    }
    return ar;
};
var __spreadArray = (this && this.__spreadArray) || function (to, from, pack) {
    if (pack || arguments.length === 2) for (var i = 0, l = from.length, ar; i < l; i++) {
        if (ar || !(i in from)) {
            if (!ar) ar = Array.prototype.slice.call(from, 0, i);
            ar[i] = from[i];
        }
    }
    return to.concat(ar || Array.prototype.slice.call(from));
};
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.isReactComponent = exports.usePortalsLifecycle = exports.ReactPart = exports.ReactPartContext = void 0;
var react_1 = __importDefault(require("react"));
var react_dom_1 = __importDefault(require("react-dom"));
var dockview_core_1 = require("dockview-core");
/**
 * This component is intended to interface between vanilla-js and React hence we need to be
 * creative in how we update props.
 * A ref of the component is exposed with an update method; which when called stores the props
 * as a ref within this component and forcefully triggers a re-render of the component using
 * the ref of props we just set on the renderered component as the props passed to the inner
 * component
 */
var ReactComponentBridge = function (props, ref) {
    var _a = __read(react_1.default.useState(), 2), _ = _a[0], triggerRender = _a[1];
    var _props = react_1.default.useRef(props.componentProps);
    react_1.default.useImperativeHandle(ref, function () { return ({
        update: function (componentProps) {
            _props.current = __assign(__assign({}, _props.current), componentProps);
            /**
             * setting a arbitrary piece of state within this component will
             * trigger a re-render.
             * we use this rather than updating through a prop since we can
             * pass a ref into the vanilla-js world.
             */
            triggerRender(Date.now());
        },
    }); }, []);
    return react_1.default.createElement(props.component, _props.current);
};
ReactComponentBridge.displayName = 'DockviewReactJsBridge';
/**
 * Since we are storing the React.Portal references in a rendered array they
 * require a key property like any other React element rendered in an array
 * to prevent excessive re-rendering
 */
var uniquePortalKeyGenerator = (function () {
    var value = 1;
    return { next: function () { return "dockview_react_portal_key_".concat((value++).toString()); } };
})();
exports.ReactPartContext = react_1.default.createContext({});
var ReactPart = /** @class */ (function () {
    function ReactPart(parent, portalStore, component, parameters, context) {
        this.parent = parent;
        this.portalStore = portalStore;
        this.component = component;
        this.parameters = parameters;
        this.context = context;
        this._initialProps = {};
        this.disposed = false;
        this.createPortal();
    }
    ReactPart.prototype.update = function (props) {
        if (this.disposed) {
            throw new Error('invalid operation: resource is already disposed');
        }
        if (!this.componentInstance) {
            // if the component is yet to be mounted store the props
            this._initialProps = __assign(__assign({}, this._initialProps), props);
        }
        else {
            this.componentInstance.update(props);
        }
    };
    ReactPart.prototype.createPortal = function () {
        var _this = this;
        if (this.disposed) {
            throw new Error('invalid operation: resource is already disposed');
        }
        if (!isReactComponent(this.component)) {
            /**
             * we know this isn't a React.FunctionComponent so throw an error here.
             * if we do not intercept then React library will throw a very obsure error
             * for the same reason... at least at this point we will emit a sensible stacktrace.
             */
            throw new Error('Dockview: Only React.memo(...), React.ForwardRef(...) and functional components are accepted as components');
        }
        var bridgeComponent = react_1.default.createElement(react_1.default.forwardRef(ReactComponentBridge), {
            component: this
                .component,
            componentProps: this.parameters,
            ref: function (element) {
                _this.componentInstance = element;
                if (Object.keys(_this._initialProps).length > 0) {
                    _this.componentInstance.update(_this._initialProps);
                    _this._initialProps = {}; // don't keep a reference to the users object once no longer required
                }
            },
        });
        var node = this.context
            ? react_1.default.createElement(exports.ReactPartContext.Provider, { value: this.context }, bridgeComponent)
            : bridgeComponent;
        var portal = react_dom_1.default.createPortal(node, this.parent, uniquePortalKeyGenerator.next());
        this.ref = {
            portal: portal,
            disposable: this.portalStore.addPortal(portal),
        };
    };
    ReactPart.prototype.dispose = function () {
        var _a;
        (_a = this.ref) === null || _a === void 0 ? void 0 : _a.disposable.dispose();
        this.disposed = true;
    };
    return ReactPart;
}());
exports.ReactPart = ReactPart;
/**
 * A React Hook that returns an array of portals to be rendered by the user of this hook
 * and a disposable function to add a portal. Calling dispose removes this portal from the
 * portal array
 */
var usePortalsLifecycle = function () {
    var _a = __read(react_1.default.useState([]), 2), portals = _a[0], setPortals = _a[1];
    react_1.default.useDebugValue("Portal count: ".concat(portals.length));
    var addPortal = react_1.default.useCallback(function (portal) {
        setPortals(function (existingPortals) { return __spreadArray(__spreadArray([], __read(existingPortals), false), [portal], false); });
        var disposed = false;
        return dockview_core_1.DockviewDisposable.from(function () {
            if (disposed) {
                throw new Error('invalid operation: resource already disposed');
            }
            disposed = true;
            setPortals(function (existingPortals) {
                return existingPortals.filter(function (p) { return p !== portal; });
            });
        });
    }, []);
    return [portals, addPortal];
};
exports.usePortalsLifecycle = usePortalsLifecycle;
function isReactComponent(component) {
    /**
     * Yes, we could use "react-is" but that would introduce an unwanted peer dependency
     * so for now we will check in a rather crude fashion...
     */
    return (typeof component === 'function' /** Functional Componnts */ ||
        !!(component === null || component === void 0 ? void 0 : component.$$typeof) /** React.memo(...) Components */);
}
exports.isReactComponent = isReactComponent;
