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
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.GridviewReact = void 0;
var react_1 = __importDefault(require("react"));
var dockview_core_1 = require("dockview-core");
var view_1 = require("./view");
var react_2 = require("../react");
function extractCoreOptions(props) {
    var coreOptions = dockview_core_1.PROPERTY_KEYS_GRIDVIEW.reduce(function (obj, key) {
        if (key in props) {
            obj[key] = props[key];
        }
        return obj;
    }, {});
    return coreOptions;
}
exports.GridviewReact = react_1.default.forwardRef(function (props, ref) {
    var domRef = react_1.default.useRef(null);
    var gridviewRef = react_1.default.useRef();
    var _a = __read((0, react_2.usePortalsLifecycle)(), 2), portals = _a[0], addPortal = _a[1];
    react_1.default.useImperativeHandle(ref, function () { return domRef.current; }, []);
    var prevProps = react_1.default.useRef({});
    react_1.default.useEffect(function () {
        var changes = {};
        dockview_core_1.PROPERTY_KEYS_GRIDVIEW.forEach(function (propKey) {
            var key = propKey;
            var propValue = props[key];
            if (key in props && propValue !== prevProps.current[key]) {
                changes[key] = propValue;
            }
        });
        if (gridviewRef.current) {
            gridviewRef.current.updateOptions(changes);
        }
        else {
            // not yet fully initialized
        }
        prevProps.current = props;
    }, dockview_core_1.PROPERTY_KEYS_GRIDVIEW.map(function (key) { return props[key]; }));
    react_1.default.useEffect(function () {
        if (!domRef.current) {
            return function () {
                // noop
            };
        }
        var frameworkOptions = {
            createComponent: function (options) {
                return new view_1.ReactGridPanelView(options.id, options.name, props.components[options.name], { addPortal: addPortal });
            },
        };
        var api = (0, dockview_core_1.createGridview)(domRef.current, __assign(__assign({}, extractCoreOptions(props)), frameworkOptions));
        var _a = domRef.current, clientWidth = _a.clientWidth, clientHeight = _a.clientHeight;
        api.layout(clientWidth, clientHeight);
        if (props.onReady) {
            props.onReady({ api: api });
        }
        gridviewRef.current = api;
        return function () {
            gridviewRef.current = undefined;
            api.dispose();
        };
    }, []);
    react_1.default.useEffect(function () {
        if (!gridviewRef.current) {
            return;
        }
        gridviewRef.current.updateOptions({
            createComponent: function (options) {
                return new view_1.ReactGridPanelView(options.id, options.name, props.components[options.name], { addPortal: addPortal });
            },
        });
    }, [props.components]);
    return (react_1.default.createElement("div", { style: { height: '100%', width: '100%' }, ref: domRef }, portals));
});
exports.GridviewReact.displayName = 'GridviewComponent';
