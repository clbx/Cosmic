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
var __rest = (this && this.__rest) || function (s, e) {
    var t = {};
    for (var p in s) if (Object.prototype.hasOwnProperty.call(s, p) && e.indexOf(p) < 0)
        t[p] = s[p];
    if (s != null && typeof Object.getOwnPropertySymbols === "function")
        for (var i = 0, p = Object.getOwnPropertySymbols(s); i < p.length; i++) {
            if (e.indexOf(p[i]) < 0 && Object.prototype.propertyIsEnumerable.call(s, p[i]))
                t[p[i]] = s[p[i]];
        }
    return t;
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
exports.DockviewDefaultTab = void 0;
var react_1 = __importDefault(require("react"));
var svg_1 = require("../svg");
function useTitle(api) {
    var _a = __read(react_1.default.useState(api.title), 2), title = _a[0], setTitle = _a[1];
    react_1.default.useEffect(function () {
        var disposable = api.onDidTitleChange(function (event) {
            setTitle(event.title);
        });
        // Depending on the order in which React effects are run, the title may already be out of sync (cf. issue #1003).
        if (title !== api.title) {
            setTitle(api.title);
        }
        return function () {
            disposable.dispose();
        };
    }, [api]);
    return title;
}
var DockviewDefaultTab = function (_a) {
    var api = _a.api, _containerApi = _a.containerApi, _params = _a.params, hideClose = _a.hideClose, closeActionOverride = _a.closeActionOverride, onPointerDown = _a.onPointerDown, onPointerUp = _a.onPointerUp, onPointerLeave = _a.onPointerLeave, tabLocation = _a.tabLocation, rest = __rest(_a, ["api", "containerApi", "params", "hideClose", "closeActionOverride", "onPointerDown", "onPointerUp", "onPointerLeave", "tabLocation"]);
    var title = useTitle(api);
    var isMiddleMouseButton = react_1.default.useRef(false);
    var onClose = react_1.default.useCallback(function (event) {
        event.preventDefault();
        if (closeActionOverride) {
            closeActionOverride();
        }
        else {
            api.close();
        }
    }, [api, closeActionOverride]);
    var onBtnPointerDown = react_1.default.useCallback(function (event) {
        event.preventDefault();
    }, []);
    var _onPointerDown = react_1.default.useCallback(function (event) {
        isMiddleMouseButton.current = event.button === 1;
        onPointerDown === null || onPointerDown === void 0 ? void 0 : onPointerDown(event);
    }, [onPointerDown]);
    var _onPointerUp = react_1.default.useCallback(function (event) {
        if (isMiddleMouseButton && event.button === 1 && !hideClose) {
            isMiddleMouseButton.current = false;
            onClose(event);
        }
        onPointerUp === null || onPointerUp === void 0 ? void 0 : onPointerUp(event);
    }, [onPointerUp, onClose, hideClose]);
    var _onPointerLeave = react_1.default.useCallback(function (event) {
        isMiddleMouseButton.current = false;
        onPointerLeave === null || onPointerLeave === void 0 ? void 0 : onPointerLeave(event);
    }, [onPointerLeave]);
    return (react_1.default.createElement("div", __assign({ "data-testid": "dockview-dv-default-tab" }, rest, { onPointerDown: _onPointerDown, onPointerUp: _onPointerUp, onPointerLeave: _onPointerLeave, className: "dv-default-tab" }),
        react_1.default.createElement("span", { className: "dv-default-tab-content" }, title),
        !hideClose && (react_1.default.createElement("div", { className: "dv-default-tab-action", onPointerDown: onBtnPointerDown, onClick: onClose },
            react_1.default.createElement(svg_1.CloseButton, null)))));
};
exports.DockviewDefaultTab = DockviewDefaultTab;
