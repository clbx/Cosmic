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
import React from 'react';
import { CloseButton } from '../svg';
function useTitle(api) {
    const [title, setTitle] = React.useState(api.title);
    React.useEffect(() => {
        const disposable = api.onDidTitleChange((event) => {
            setTitle(event.title);
        });
        // Depending on the order in which React effects are run, the title may already be out of sync (cf. issue #1003).
        if (title !== api.title) {
            setTitle(api.title);
        }
        return () => {
            disposable.dispose();
        };
    }, [api]);
    return title;
}
export const DockviewDefaultTab = (_a) => {
    var { api, containerApi: _containerApi, params: _params, hideClose, closeActionOverride, onPointerDown, onPointerUp, onPointerLeave, tabLocation } = _a, rest = __rest(_a, ["api", "containerApi", "params", "hideClose", "closeActionOverride", "onPointerDown", "onPointerUp", "onPointerLeave", "tabLocation"]);
    const title = useTitle(api);
    const isMiddleMouseButton = React.useRef(false);
    const onClose = React.useCallback((event) => {
        event.preventDefault();
        if (closeActionOverride) {
            closeActionOverride();
        }
        else {
            api.close();
        }
    }, [api, closeActionOverride]);
    const onBtnPointerDown = React.useCallback((event) => {
        event.preventDefault();
    }, []);
    const _onPointerDown = React.useCallback((event) => {
        isMiddleMouseButton.current = event.button === 1;
        onPointerDown === null || onPointerDown === void 0 ? void 0 : onPointerDown(event);
    }, [onPointerDown]);
    const _onPointerUp = React.useCallback((event) => {
        if (isMiddleMouseButton && event.button === 1 && !hideClose) {
            isMiddleMouseButton.current = false;
            onClose(event);
        }
        onPointerUp === null || onPointerUp === void 0 ? void 0 : onPointerUp(event);
    }, [onPointerUp, onClose, hideClose]);
    const _onPointerLeave = React.useCallback((event) => {
        isMiddleMouseButton.current = false;
        onPointerLeave === null || onPointerLeave === void 0 ? void 0 : onPointerLeave(event);
    }, [onPointerLeave]);
    return (React.createElement("div", Object.assign({ "data-testid": "dockview-dv-default-tab" }, rest, { onPointerDown: _onPointerDown, onPointerUp: _onPointerUp, onPointerLeave: _onPointerLeave, className: "dv-default-tab" }),
        React.createElement("span", { className: "dv-default-tab-content" }, title),
        !hideClose && (React.createElement("div", { className: "dv-default-tab-action", onPointerDown: onBtnPointerDown, onClick: onClose },
            React.createElement(CloseButton, null)))));
};
